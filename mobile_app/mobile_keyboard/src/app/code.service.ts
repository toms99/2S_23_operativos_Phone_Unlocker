import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class CodeService {
  currentInsertedCode = "";
  expectedCode = "6523"

  constructor() {
   }
}
