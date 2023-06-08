import { Component, NgModule, OnInit } from '@angular/core';
import { CommonModule } from '@angular/common';
import { FormsModule } from '@angular/forms';
import { AlertController, IonicModule, IonicSafeString } from '@ionic/angular';
import { CodeService } from '../code.service';
import { LongPressModule } from 'ionic-long-press';
import { DirectivesModule } from '../directives/directives.module';

@Component({
  selector: 'app-t1',
  templateUrl: './t1.page.html',
  styleUrls: ['./t1.page.scss'],
  standalone: true,
  imports: [IonicModule, CommonModule, FormsModule, DirectivesModule]
})
  
 
export class T1Page implements OnInit {
  public alertButtons = ['OK'];
  disableButton = false;
  currentInsertedCode = ""
  keyboardSelected = 2;
  delay = 1000;
  pressTimer: any;
  pressedTimes = 0;
  constructor(private codeService: CodeService, public alertController: AlertController) {
  }

  ngOnInit() {
  }

  setKeyoboard(e: Event) {
    //@ts-ignore
    this.keyboardSelected = e.detail.value;
  }

  handlePress() {
    console.log("PRESS");
  }



  async setNumber(num: number) {
    this.pressedTimes = this.pressedTimes + 1;
    this.disableButton = true;
    let previous_code = this.currentInsertedCode;
    if (this.pressedTimes == 1) {
      this.currentInsertedCode = this.currentInsertedCode.concat(num.toString());
    }
    // Espera 0.5 segundos utilizando una promesa
    await new Promise(resolve => setTimeout(resolve, this.delay));

    this.disableButton = false; // Vuelve a habilitar el botón
    if (this.currentInsertedCode.length > previous_code.length + 1) {
      this.currentInsertedCode.slice(0, this.currentInsertedCode.length - 1)
    }
    this.pressedTimes = 0;
  }

  async verifyCode() {
    this.disableButton = true;
    // Espera 0.5 segundos utilizando una promesa
    console.log(this.currentInsertedCode)
    if (this.currentInsertedCode == this.codeService.expectedCode) {
      this.codeService.currentInsertedCode = this.currentInsertedCode
      this.showOk()
    } else {
      this.clearCode()
      this.showAlert()
    }
    await new Promise(resolve => setTimeout(resolve, this.delay));
    this.disableButton = false; // Vuelve a habilitar el botón

  }

  async clearCode() {
    this.disableButton = true;
    this.currentInsertedCode = ''
    await new Promise(resolve => setTimeout(resolve, this.delay));
    this.disableButton = false; // Vuelve a habilitar el botón
  }

  async showOk() {
    let msg = new IonicSafeString('<img src="../../assets/giphy.gif">');
    console.log(msg)
    let confirmationAlert = await this.alertController.create({
      header: 'Great :)',
      subHeader: 'Code asserted.',
      message: msg,
      buttons: ['OK']
    })

    await confirmationAlert.present();
  }

  showAlert() {

    this.alertController.create({
      header: 'Error :(',
      subHeader: 'Wrong pin!',
      message: 'Please type the correct value.',
      buttons: ['OK']
    }).then(res => {

      res.present();

    });

  }
}
